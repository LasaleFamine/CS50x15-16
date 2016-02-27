

<div class="container">
    <div class="row">
        <div class="panel panel-primary filterable">
            <div class="panel-heading">
                <h3 class="panel-title">Portfolio</h3>
            </div>
             <table class="table table-striped">
                <thead>
                    <tr class="filters">
                        <?php foreach($tableHeaders as $head): ?>
                            <th> <?= $head ?> </th>
                        <?php endforeach ?>
                    </tr>
                </thead>
                <tbody>
                    <?php foreach($userProp as $prop): ?>
                            <tr>
                                <?php foreach($prop as $value): ?>
                                    <td><?= $value ?></td>
                                <?php endforeach ?>
                            </tr>
                    <?php endforeach ?>
                    
                    <tr>
                        <td colspan="<?= count($tableHeaders) - 1 ?>">CASH</td>
                        <td><?= $cash ?></td>
                    </tr>
                </tbody>
            </table>
        </div>
    </div>
</div>